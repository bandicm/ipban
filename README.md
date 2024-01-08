
# Library for the fail-ban function

IPBan is a C++ library for banning IP addresses in case of consecutive errors.

## Features

- Object oriented
- Thread safe
- Internal database of banned addresses as a protection mechanism against irreversible ban
- Automatic unbanning
- Manual fail and unfail methods
- White list of IP addresses
- It is adjustable: database file location, tracking time for errors, number of attempts before ban, ban duration.
## Installation

This library also requires my exec library to work, download the latest release:

https://git.bitelex.co/marcelb/exec or https://github.com/bandicm/exec

Then download the latest backend of these libraries and unzip to the same location. You can turn it on with:

```
#include "cppurl/lib/cppurl.hpp"
using namespace marcelb;
```

## Usage

```c++
//init
ipban myban(30); // 30 minutes
// add white list string or vector<string>
myban.add_white_list("10.0.8.1");
// ban now ip
myban.ban("10.0.8.2");
// fail ip
myban.fail("10.0.8.3");
// unfail ip
myban.unfail("10.0.8.3");

```
## License

[APACHE 2.0](http://www.apache.org/licenses/LICENSE-2.0/)


## Support & Feedback

For support and any feedback, contact the address: marcelb96@yahoo.com.

## Contributing

Contributions are always welcome!

Feel free to fork and start working with or without a later pull request. Or contact for suggest and request an option.

