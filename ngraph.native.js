var addon = require('bindings')('force-layout');

const run = (nodes, edges, iterations) => {

  const nodeTArray = Int32Array.from(nodes);
  const edgeTArray = Int32Array.from(Array.prototype.concat(...edges));

  const rawTArrayResult = addon.runLayout(nodeTArray, edgeTArray, iterations); // 'world'

  const rawResult = Array.from(rawTArrayResult);

  const result = [];
  for(let i=0; i<nodes.length; i+=1) {
    result.push({
      id: nodes[i],
      x: rawResult[(i*3)+1],
      y: rawResult[(i*3)+2]
    })
  }

  return result;
}

module.exports = { run };
