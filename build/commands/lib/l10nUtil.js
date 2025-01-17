/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
 * This file manages the following:
 * - Lists of files needed to be translated (Which is all top level GRD and JSON files)
 * - All mappings for auto-generated adrbrowsiel files from the associated Chromium files.
 * - Top level global string replacements, such as replacing Chromium with adrbrowsiel
 */

const path = require('path')
const fs = require('fs')
const chalk = require('chalk')
const { JSDOM } = require("jsdom")
const rootDir = require('./root')

// Change to `true` for verbose console log output of GRD traversal
const verboseLogFindGrd = false
const srcDir = path.join(rootDir, 'src')

// chromium_strings.grd and any of its parts files that we track localization for in transifex
// These map to adrbrowsiel/app/resources/chromium_strings*.xtb
const chromiumStringsPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'chromium_strings.grd'))
const adrbrowsielStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'app', 'adrbrowsiel_strings.grd'))
const chromiumSettingsPartPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'settings_chromium_strings.grdp'))
const adrbrowsielSettingsPartPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'app', 'settings_adrbrowsiel_strings.grdp'))

//Replace android strings.
const androidChromeStringsPath = path.resolve(path.join(srcDir, 'chrome', 'browser', 'ui', 'android', 'strings', 'android_chrome_strings.grd'))
const adrbrowsielAndroidChromeStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'browser', 'ui', 'android', 'strings', 'android_chrome_strings.grd'))

// component_chromium_strings.grd and any of its parts files that we track localization for in transifex
// These map to adrbrowsiel/app/strings/components_chromium_strings*.xtb
const chromiumComponentsChromiumStringsPath = path.resolve(path.join(srcDir, 'components', 'components_chromium_strings.grd'))
const adrbrowsielComponentsadrbrowsielStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'components', 'components_adrbrowsiel_strings.grd'))

// components/component_strings.grd and any of its parts files that we track localization for in transifex
// These map to adrbrowsiel/components/component_strings*.xtb
const chromiumComponentsStringsPath = path.resolve(path.join(srcDir, 'components', 'components_strings.grd'))
const adrbrowsielComponentsStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'components', 'components_strings.grd'))

// generated_resources.grd and any of its parts files that we track localization for in transifex
// There is also chromeos_strings.grdp, but we don't need to track it here because it is explicitly skipped in transifex.py
// These map to adrbrowsiel/app/resources/generated_resoruces*.xtb
const chromiumGeneratedResourcesPath = path.resolve(path.join(srcDir, 'chrome', 'app', 'generated_resources.grd'))
const adrbrowsielGeneratedResourcesPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'app', 'generated_resources.grd'))
const chromiumGeneratedResourcesExcludes = new Set(["chromeos_strings.grdp"])

// The following are not generated files but still need to be tracked so they get sent to transifex
// These xtb files don't need to be copied anywhere.
// adrbrowsiel_generated_resources.grd maps to adrbrowsiel/app/resources/adrbrowsiel_generated_resources*.xtb,
// adrbrowsiel_components_strings.grd maps to adrbrowsiel/components/resources/strings/adrbrowsiel_components_resources*.xtb
// messages.json localization is handled inside of adrbrowsiel-extension.
const adrbrowsielSpecificGeneratedResourcesPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'app', 'adrbrowsiel_generated_resources.grd'))
const adrbrowsielResourcesComponentsStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'components', 'resources', 'adrbrowsiel_components_strings.grd'))
const adrbrowsielExtensionMessagesPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'components', 'adrbrowsiel_extension', 'extension', 'adrbrowsiel_extension', '_locales', 'en_US', 'messages.json'))
const adrbrowsielRewardsExtensionMessagesPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'components', 'adrbrowsiel_rewards', 'resources', 'extension', 'adrbrowsiel_rewards', '_locales', 'en_US', 'messages.json'))
const adrbrowsielAndroidadrbrowsielStringsPath = path.resolve(path.join(srcDir, 'adrbrowsiel', 'browser', 'ui', 'android', 'strings', 'android_adrbrowsiel_strings.grd'))

// Helper function to find all grdp parts in a grd.
function getGrdPartsFromGrd(path) {
  const grd = new JSDOM(fs.readFileSync(path, 'utf8'))
  const partTags = grd.window.document.getElementsByTagName("part")
  let parts = new Array()
  for (const partTag of partTags) {
    parts.push(partTag.getAttribute('file'));
  }
  return parts
}

// Helper function to create a mapping for grd and all of its grdp parts.
function addGrd(chromiumPath, adrbrowsielPath, exclude = new Set()) {
  if (verboseLogFindGrd)
    console.log("Adding mappings for GRD: " + chromiumPath)
  if (!fs.existsSync(chromiumPath)) {
    const err = new Error(`addGrd: Error. File not found at path "${chromiumPath}"`)
    console.error(err)
    throw err
  }
  let mapping = {}
  // Add grd parts before grd because chromium-rebase-l10n.py expects them to be
  // processed first.
  const grdps = getGrdPartsFromGrd(chromiumPath)
  if (grdps.length) {
    const chromiumDir = path.dirname(chromiumPath)
    const adrbrowsielDir = path.dirname(adrbrowsielPath)
    for (const grdp of grdps) {
      if (exclude.has(grdp)) {
        continue
      }
      mapping[path.resolve(path.join(chromiumDir, grdp))] = path.resolve(path.join(adrbrowsielDir, grdp))
    }
    if (verboseLogFindGrd)
      console.log("  - Added " + (Object.keys(mapping).length - 1) + " GRDP.")
  }
  mapping[chromiumPath] = adrbrowsielPath
  return mapping
}

// Helper functions that's, for a given pair of chromium to adrbrowsiel GRD mapping
// from the supplied map, determines which GRDP parts are no longer present in
// the chromium GRD file.
function getRemovedGRDParts(mapping) {
  let removedMap = new Map()
  for (const [sourcePath, destPath] of Object.entries(mapping)) {
    if (path.extname(destPath) === ".grd") {
      const adrbrowsielGRDPs = getGrdPartsFromGrd(destPath)
      const chromiumGRDPs = getGrdPartsFromGrd(sourcePath)
      let removed = new Set()
      for (let i = 0; i < adrbrowsielGRDPs.length; i++) {
        if (!chromiumGRDPs.includes(adrbrowsielGRDPs[i])) {
          removed.add(adrbrowsielGRDPs[i])
        }
      }
      if (removed.size) {
        removedMap.set(destPath, removed)
      }
    }
  }
  return removedMap
}

// Add all GRD mappings here.
function getAutoGeneratedGrdMappings() {
  if (typeof(getAutoGeneratedGrdMappings.mappings) === 'undefined') {
    console.log(chalk.italic('Recursing through GRD to find GRDP files...'))
    // adrbrowsiel specific only grd and grdp files should NOT be added.
    // Using AddGrd will add GRD and all of its GRDPs.
    getAutoGeneratedGrdMappings.mappings = {
      ...addGrd(chromiumComponentsStringsPath, adrbrowsielComponentsStringsPath),
      ...addGrd(chromiumGeneratedResourcesPath, adrbrowsielGeneratedResourcesPath, chromiumGeneratedResourcesExcludes),
      ...addGrd(androidChromeStringsPath, adrbrowsielAndroidChromeStringsPath)
    }
    console.log(chalk.italic('Done recursing through GRD to find GRDP files.'))
  }
  return getAutoGeneratedGrdMappings.mappings
}

function getChromiumToAutoGeneratedadrbrowsielMapping() {
  if (typeof(getChromiumToAutoGeneratedadrbrowsielMapping.mapping) === 'undefined') {
    // When adding new grd or grdp files, never add a grdp part path without a 
    // parent grd path, but add the grd parts to the mapping before the parent
    // grd, becase chromium-rebase-l10n.py expects them to be processed first.
    // Group them with a leading and trailing newline to keep this file organized.
    // The first 3 are added explicitly because we change the file names.
    getChromiumToAutoGeneratedadrbrowsielMapping.mapping = {
      [chromiumSettingsPartPath]: adrbrowsielSettingsPartPath,
      [chromiumStringsPath]: adrbrowsielStringsPath,

      [chromiumComponentsChromiumStringsPath]: adrbrowsielComponentsadrbrowsielStringsPath,

      ...getAutoGeneratedGrdMappings()
    }
  }
  return getChromiumToAutoGeneratedadrbrowsielMapping.mapping
}

const l10nUtil = {
  // Same as with chromiumToAutoGeneratedadrbrowsielMapping but maps in the opposite direction
  getAutoGeneratedadrbrowsielToChromiumMapping: () => {
    if (typeof(l10nUtil.getAutoGeneratedadrbrowsielToChromiumMapping.mapping) === 'undefined') {
      const chromiumToAutoGeneratedadrbrowsielMapping = getChromiumToAutoGeneratedadrbrowsielMapping()
      l10nUtil.getAutoGeneratedadrbrowsielToChromiumMapping.mapping = Object.keys(
        chromiumToAutoGeneratedadrbrowsielMapping).reduce((obj, key) => (
          { ...obj, [chromiumToAutoGeneratedadrbrowsielMapping[key]]: key }), {})
    }
    return l10nUtil.getAutoGeneratedadrbrowsielToChromiumMapping.mapping
  },

  // All paths which are generated
  getadrbrowsielAutoGeneratedPaths: () => {
    return Object.values(getChromiumToAutoGeneratedadrbrowsielMapping())
  },

  // All paths which are not generated
  getadrbrowsielNonGeneratedPaths: () => {
    if (typeof(l10nUtil.getadrbrowsielNonGeneratedPaths.paths) === 'undefined') {
      l10nUtil.getadrbrowsielNonGeneratedPaths.paths = [
        adrbrowsielSpecificGeneratedResourcesPath,
        adrbrowsielResourcesComponentsStringsPath,
        adrbrowsielExtensionMessagesPath,
        adrbrowsielRewardsExtensionMessagesPath,
        adrbrowsielAndroidadrbrowsielStringsPath
      ]
    }
    return l10nUtil.getadrbrowsielNonGeneratedPaths.paths
  },

  // adrbrowsiel specific strings and Chromium mapped adrbrowsiel strings will be here.
  // But you only need to add the adrbrowsiel specific strings manually here.
  getAlladrbrowsielPaths: () => {
    return l10nUtil.getadrbrowsielNonGeneratedPaths().concat(l10nUtil.getadrbrowsielAutoGeneratedPaths())
  },

  // Get all GRD and JSON paths whether they are generatd or not
  // Push and pull scripts for l10n use this.
  // Transifex manages files per grd and not per grd or grdp.
  // This is because only 1 xtb is created per grd per locale even if it has multiple grdp files.
  getadrbrowsielTopLevelPaths: () => {
    return l10nUtil.getAlladrbrowsielPaths().filter((x) => ['grd', 'json'].includes(x.split('.').pop()))
  },

// Helper function to retrieve ethereum-remote-client paths relative
// to the adrbrowsiel paths
  getEthereumRemoteClientPaths: (extensionPath) => {
    let basePath = extensionPath
    if (!basePath) {
      basePath = '../../../ethereum-remote-client'
    }

    return [
      `${basePath}/app/_locales/en/messages.json`
    ]
  },

// Helper function to retrieve Greaselion script paths relative to the
// adrbrowsiel paths.
//
// Greaselion.json consists of an array of Greaselion rules,
// specifying scripts to inject into given sites based on certain
// preconditions. If the rule contains a "messages" key, then the
// script contains user-visible strings that require translation. This
// helper function gathers those messages.json files for transmission
// to Transifex.
  getGreaselionScriptPaths: (extensionPath) => {
    let basePath = extensionPath
    if (!basePath) {
      basePath = '../../../adrbrowsiel-site-specific-scripts'
    }

    const jsonContent = fs.readFileSync(`${basePath}/Greaselion.json`, 'utf8')
    if (!jsonContent) {
      console.error('Missing Greaselion.json')
      return []
    }

    const greaselionRules = JSON.parse(jsonContent)
    if (!greaselionRules) {
      console.error('Malformed Greaselion.json')
      return []
    }

    let paths = []
    greaselionRules.forEach((rule) => {
      if (rule.messages) {
        paths.push(`${basePath}/${rule.messages}/en_US/messages.json`)
      }
    })

    return paths
  },

  // Helper function to pretty print removed GRDP file names.
  logRemovedGRDParts: (mapping) => {
    if (mapping.size) {
      console.log("\n**************************************************************************")
      console.log("The following GRDP files are no longer in the corresponding Chromium GRDs:\n")
      for (const [grd, grdps] of mapping.entries()) {
        console.log("  From " + grd + ":")
        for (const grdp of grdps) {
          console.log("    - " + grdp)
        }
      }
    }
  },

  // This simply reads Chromium files that are passed to it and replaces branding strings
  // with adrbrowsiel specific branding strings.
  // Do not use this for filtering XML, instead use chromium-rebase-l10n.py.
  // Only add idempotent replacements here (i.e. don't append replace A with AX here)
  rebaseadrbrowsielStringFilesOnChromiumL10nFiles: async (path) => {
    const removedMap = getRemovedGRDParts(getAutoGeneratedGrdMappings())
    const ops = Object.entries(getChromiumToAutoGeneratedadrbrowsielMapping()).map(async ([sourcePath, destPath]) => {
      let contents = await new Promise(resolve => fs.readFile(sourcePath, 'utf8', (err, data) => resolve(data)))
      await new Promise(resolve => fs.writeFile(destPath, contents, 'utf8', resolve))
    })
    await Promise.all(ops)
    return removedMap
  },
}  // const l10nUtil

module.exports = l10nUtil
