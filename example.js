const caki = require('./index');
const fs = require('fs');

const fileContent = fs.readFileSync('example.txt');
const example = caki.parse(fileContent);
console.log(JSON.stringify(example, undefined, 2));