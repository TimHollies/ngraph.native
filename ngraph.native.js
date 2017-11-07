var addon = require('bindings')('force-layout');

console.log(addon.runLayout(Int32Array.from([2, 1]), Int32Array.from([1, 2, 2, 1]))); // 'world'
