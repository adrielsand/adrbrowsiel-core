// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

const fs = require('fs')
const program = require('commander')
const config = require('../lib/config')
const util = require('../lib/util')
const Log = require('../lib/sync/logging')
const os = require('os')

program
  .version(process.env.npm_package_version)
  .arguments('[ref]')
  .option('--gclient_file <file>', 'gclient config file location')
  .option('--gclient_verbose', 'verbose output for gclient')
  .option('--run_hooks', 'This flag is deprecated and no longer has any effect')
  .option('--run_sync', 'This flag is deprecated and no longer has any effect')
  .option('--target_os <target_os>', 'target OS')
  .option('--target_arch <target_arch>', 'target architecture')
  .option('--target_android_base <target_android_base>', 'target Android OS level for apk or aab (classic, modern, mono)')
  .option('--init', 'initialize all dependencies')
  .option('--all', 'This flag is deprecated and no longer has any effect')
  .option('--force', 'force reset all projects to origin/ref')
  .option('--create', 'create a new branch if needed for [ref]')

const installDepotTools = (options = config.defaultOptions) => {
  options.cwd = config.adrbrowsielCoreDir

  if (!fs.existsSync(config.depotToolsDir)) {
    Log.progress('Install Depot Tools...')
    fs.mkdirSync(config.depotToolsDir)
    util.run('git', ['-C', config.depotToolsDir, 'clone', 'https://chromium.googlesource.com/chromium/tools/depot_tools.git', '.'], options)
  }

  Log.progress('Fixup Depot Tools...')
  // fixup depot tools after update
  if (process.platform !== 'win32') {
    util.run('git', ['-C', config.depotToolsDir, 'clean', '-fxd'], options)
    util.run('git', ['-C', config.depotToolsDir, 'reset', '--hard', 'HEAD'], options)
  } else {
    // On Windows:
    // When depot_tools are already installed they redirect git to their own
    // version which resides in a bootstrap-*_bin directory. So when we try to
    // do git clean -fxd we fail because the git executable is in use in that
    // directory. Get around that by using regular git.
    let git_exes = util.run('where', ['git'], {shell: true})
    let git_exe = '"' + git_exes.stdout.toString().split(os.EOL)[0] + '"'
    if (git_exe === '""') git_exe = 'git'
    util.run(git_exe, ['-C', config.depotToolsDir, 'clean', '-fxd'], options)
    util.run(git_exe, ['-C', config.depotToolsDir, 'reset', '--hard', 'HEAD'], options)
  }
  Log.progress('Done Depot Tools...')
}

async function RunCommand () {
  program.parse(process.argv)
  config.update(program)

  if (program.all || program.run_hooks || program.run_sync) {
    Log.warn('--all, --run_hooks and --run_sync are deprecated. Will behave as if flag was not passed. Please update your command to `npm run sync` in the future.')
  }

  if (program.init || !fs.existsSync(config.depotToolsDir)) {
    installDepotTools()
  }

  if (program.init) {
    util.buildGClientConfig()
  }

  let adrbrowsielCoreRef = program.args[0]
  if (!adrbrowsielCoreRef) {
    adrbrowsielCoreRef = program.init ? config.getProjectVersion('adrbrowsiel-core') : null
  }

  if (adrbrowsielCoreRef || program.init || program.force) {
    // we're doing a reset of adrbrowsiel-core so try to stash any changes
    Log.progress('Stashing any local changes')
    util.runGit(config.adrbrowsielCoreDir, ['stash'], true)
  }

  if (adrbrowsielCoreRef) {
    Log.progress(`Resetting adrbrowsiel core to "${adrbrowsielCoreRef}"...`)
    // try to checkout to the right ref if possible
    util.runGit(config.adrbrowsielCoreDir, ['reset', '--hard', 'HEAD'], true)
    let checkoutResult = util.runGit(config.adrbrowsielCoreDir, ['checkout', adrbrowsielCoreRef], true)
    if (checkoutResult === null && program.create) {
      checkoutResult = util.runGit(config.adrbrowsielCoreDir, ['checkout', '-b', adrbrowsielCoreRef], true)
    }
    // Handle checkout failure
    if (checkoutResult === null) {
      Log.error('Could not checkout: ' + adrbrowsielCoreRef)
    }
    // Checkout was successful
    const adrbrowsielCoreSha = util.runGit(config.adrbrowsielCoreDir, ['rev-parse', 'HEAD'])
    Log.progress(`...adrbrowsiel core is now at commit ID ${adrbrowsielCoreSha}`)
  }

  Log.progress('Running gclient sync...')
  const result = util.gclientSync(program.init || program.force, program.init, adrbrowsielCoreRef)
  const postSyncadrbrowsielCoreRef = util.getGitReadableLocalRef(config.adrbrowsielCoreDir)
  Log.status(`adrbrowsiel Core is now at ${postSyncadrbrowsielCoreRef || '[unknown]'}`)
  if (result.didUpdateChromium) {
    const postSyncChromiumRef = util.getGitReadableLocalRef(config.srcDir)
    Log.status(`Chromium is now at ${postSyncChromiumRef || '[unknown]'}`)
  }
  Log.progress('...gclient sync done')

  await util.applyPatches()

  util.gclientRunhooks()
}

Log.progress('adrbrowsiel Browser Sync starting')
RunCommand()
.then(() => {
  Log.progress('adrbrowsiel Browser Sync complete')
})
.catch((err) => {
  Log.error('adrbrowsiel Browser Sync ERROR:')
  console.error(err)
  process.exit(1)
})
