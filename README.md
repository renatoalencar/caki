# Caki

Caki is a library that provides a simple interface that reads handwritten config files.

## Quickstart

* Comments begin with a `#`
* Comments are ignored
* Empty spaces are ignored
* Empty lines are ignored
* Identifiers (keys) are `[_a-zA-Z0-9]+`
* You can use integers and floats

Comments begin with a '#', eg.:

```
# This is a comment
```

Each node is a pair of a key and a value, like:

```
<key> <value>;
```

You can declare subnodes too:

```
<key> {
	<key> <value>;
	<key> <value>;
	.
	.
	.
}
```

You must put a semicollon (`;`) at the end of line.

Example file:

```conf

# This is an example file

name "George Lucas";

login {
	username "root";
	password "123456";
}

pi 3.14159;
bits 32;

```

Then you can import and parse it:

```js
const caki = require('caki');

const example = caki.parse('example.txt');

console.log(JSON.stringify(example, undefined, 2));
```

## LICENSE

[MIT](./LICENSE)