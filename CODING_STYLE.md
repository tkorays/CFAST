# Coding Style for cfast

K&R style

name of files


name of export types
cf_xxx_t

name of internal types
xxx


name of variables
lower case



indentation ans space


comment

use `/*  */` to comment code.
`//` is not suggested

enum
enum type should follow the type name Style
enum items should use all capital letters.

macro
capital letters.


file scope global variables

start with `_g_`


create object
like cpp, use `new` and `delete` for creating and destroy objects.
use `init` and `deinit` for constructing and deconstructing objects.

a lightweight object is a object with simple structure and there is no need to create a lightweight in heap.
a lightweight only provides init and deinit interface.

if a class provides `new` and `delete`, we will not provide `init` and `deinit` functions. function of `init` and `deinit` is 
replaced by `new` and `delete`.

class functions

`cf_class_name_t`
`cf_class_name_init`
`cf_class_name_deinit`

