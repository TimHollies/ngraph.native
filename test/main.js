const { run } = require('../ngraph.native');

//smoke
let res = run([1,2],[[1,2]], 200);

console.log(res);
