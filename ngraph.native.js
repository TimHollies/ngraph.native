var addon = require('bindings')('force-layout');

const run = (nodes, edges) => {
  const rawResult = Array.from(addon.runLayout(Int32Array.from(nodes), Int32Array.from(Array.prototype.concat(...edges)))); // 'world'

  const result = [];
  for(let i=0; i<rawResult.length; i+=3) {
    result.push({
      id: "n" + rawResult[i],
      x: rawResult[i+1],
      y: rawResult[i+2]
    })
  }

  return result;
}

module.exports = { run };
