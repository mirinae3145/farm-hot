// metro.config.js
const { getDefaultConfig } = require('expo/metro-config');

const config = getDefaultConfig(__dirname);

// disable package.json “exports” so ws won’t try to pull in Node built-ins
config.resolver.unstable_enablePackageExports = false;

// alias Node core modules to browser-friendly versions
config.resolver.extraNodeModules = {
  stream: require.resolve('readable-stream'),
  /* if you need crypto too, e.g.
  crypto: require.resolve('react-native-crypto-js'),
  */
};

// if the package uses .cjs files, enable that extension too
config.resolver.sourceExts.push('cjs');

module.exports = config;
