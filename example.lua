local r = require("reglua")
print(r.rmatch("^(abc|bca) ([0-9]+)$", "abc 123"))