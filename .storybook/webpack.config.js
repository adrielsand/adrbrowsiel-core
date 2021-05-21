const path = require('path')

// Export a function. Accept the base config as the only param.
module.exports = async ({ config, mode }) => {
  // Make whatever fine-grained changes you need
  config.module.rules.push({
    test: /\.(ts|tsx)$/,
    loader: require.resolve('ts-loader'),
    exclude: /node_modules\/(?!adrbrowsiel-ui)/,
    options: {
      configFile: path.resolve(__dirname, '..', 'tsconfig-storybook.json'),
      allowTsInNodeModules: true,
      getCustomTransformers: path.join(__dirname, '../components/webpack/webpack-ts-transformers.js'),
    }
  })
  config.module.rules.push({
    test: /\.avif$/,
    loader: 'file-loader'
  })
  config.resolve.alias = {
    ...config.resolve.alias,
    'adrbrowsiel-ui': path.resolve(__dirname, '../node_modules/adrbrowsiel-ui/src'),
    // Force same styled-components module for adrbrowsiel-core and adrbrowsiel-ui
    // which ensure both repos code use the same singletons, e.g. ThemeContext.
    'styled-components': path.resolve(__dirname, '../node_modules/styled-components'),
  }
  config.resolve.extensions.push('.ts', '.tsx')
  return config
}
