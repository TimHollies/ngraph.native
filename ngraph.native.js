var addon = require('bindings')('force-layout');

const run = (nodes, edges, iterations) => {

  const nodeTArray = Int32Array.from(nodes);
  const edgeTArray = Int32Array.from(Array.prototype.concat(...edges));

  const rawTArrayResult = addon.runLayout(nodeTArray, edgeTArray, iterations); // 'world'

  console.log(rawTArrayResult)

  const rawResult = Array.from(rawTArrayResult);

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
