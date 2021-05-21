const path = require('path')

const config = require('../lib/config')
const util = require('../lib/util')
const assert = require('assert')

const getTestBinary = (suite) => {
  return (process.platform === 'win32') ? `${suite}.exe` : suite
}

const getTestsToRun = (config, suite) => {
  let testsToRun = [suite]
  if (suite === 'adrbrowsiel_unit_tests') {
    if (config.targetOS !== 'android') {
      testsToRun.push('adrbrowsiel_installer_unittests')
    } else {
      testsToRun.push('bin/run_adrbrowsiel_public_test_apk')
    }
  }
  return testsToRun
}

const test = (passthroughArgs, suite, buildConfig = config.defaultBuildConfig, options) => {
  config.buildConfig = buildConfig
  config.update(options)

  let adrbrowsielArgs = [
    '--enable-logging=stderr'
  ]

  // Android doesn't support --v
  if (config.targetOS !== 'android') {
    adrbrowsielArgs.push('--v=' + options.v)

    if (options.vmodule) {
      adrbrowsielArgs.push('--vmodule=' + options.vmodule)
    }
  }

  if (options.filter) {
    adrbrowsielArgs.push('--gtest_filter=' + options.filter)
  }

  if (options.run_disabled_tests) {
    adrbrowsielArgs.push('--gtest_also_run_disabled_tests')
  }

  if (options.output) {
    adrbrowsielArgs.push('--gtest_output=xml:' + options.output)
  }

  if (options.disable_adrbrowsiel_extension) {
    adrbrowsielArgs.push('--disable-adrbrowsiel-extension')
  }

  if (options.single_process) {
    adrbrowsielArgs.push('--single_process')
  }

  if (options.test_launcher_jobs) {
    adrbrowsielArgs.push('--test-launcher-jobs=' + options.test_launcher_jobs)
  }

  adrbrowsielArgs = adrbrowsielArgs.concat(passthroughArgs)

  // Build the tests
  if (suite === 'adrbrowsiel_unit_tests' || suite === 'adrbrowsiel_browser_tests') {
    util.run('ninja', ['-C', config.outputDir, "adrbrowsiel/test:" + suite], config.defaultOptions)
  } else {
    util.run('ninja', ['-C', config.outputDir, suite], config.defaultOptions)
  }

  if (config.targetOS === 'ios') {
    util.run(path.join(config.outputDir, "iossim"), [
      path.join(config.outputDir, `${suite}.app`),
      path.join(config.outputDir, `${suite}.app/PlugIns/${suite}_module.xctest`)
    ], config.defaultOptions)
  } else {
    // Run the tests
    getTestsToRun(config, suite).forEach((testSuite) => {
      if (options.output) {
        adrbrowsielArgs.splice(adrbrowsielArgs.indexOf('--gtest_output=xml:' + options.output), 1)
        adrbrowsielArgs.push(`--gtest_output=xml:${testSuite}.xml`)
      }
      if (config.targetOS === 'android') {
        assert(
            config.targetArch === 'x86' || options.manual_android_test_device,
            'Only x86 build can be run automatically. For other builds please run test device manually and specify manual_android_test_device flag.')
      }
      if (config.targetOS === 'android' && !options.manual_android_test_device) {
        // Specify emulator to run tests on
        adrbrowsielArgs.push(`--avd-config tools/android/avd/proto/generic_android28.textpb`)
      }
      util.run(path.join(config.outputDir, getTestBinary(testSuite)), adrbrowsielArgs, config.defaultOptions)
    })
  }
}

module.exports = test
