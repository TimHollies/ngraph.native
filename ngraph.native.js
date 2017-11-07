var addon = require('bindings')('hello');

console.log(addon.add(3, 4, Int32Array.from([10, 2]))); // 'world'


function test(points, iterations) {
  const bodies = Int32Array.from([1, 2]);
  const edges = Int32Array.from([1, 2]);
  return 0;
}
