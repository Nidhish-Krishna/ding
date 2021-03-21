
# ding
### - A Programming Language for Customizable Network Traffic Handling
**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>

> NOTE:
> * Syntax given using the curly brackets **{** and **}** are only for understanding purpose.
> * No curly brackets required when writing ding code.
> * Check `../scripts/*.ding` for sample ding codes

<br>

### How to CODE in ding:
  * Comments are given after "**!**"
  * *ONLY* single line comments
  * Indentation and usage of whitespaces, and tabspaces are allowed anywhere, while the line delimiter being the new line feed.

  * Program Divided into three blocks: **.decl:** , **.func:** , **.flow:**
  * Documentation text can be given above all the blocks
	
  * decl block will have all the variable declaration
  * NO DEFINITION is allowed in decl block
  * Declaration of variables must be as <br>`{datatype} {identifier}`

  * Multiple variables of same type can be declared with a comma <br> `{datatype} {identifier1}, {identifier2}`

  * func block will have the user defined functions
  * The func block is not handled as of now. Better leave it empty.
  * The code inside func will not be processed
	
  * flow block will contain the program execution flow
  * Definitions of variables, function calls, membership assignments, operations can be done in func block

  * ONLY variable definitions, membership assignments and function calls are handled as of now.
  * Any other requirements will be handled later.
  * Variable definitions can be done by assignments

  * IP addresses must be separated by "**.**" - `127.0.0.1`
  * MAC addresses must be separated by "**-**" - `ca-fe-ba-be-00-99`
  * Numbers will contain two parts separated by a "**:**" and ending with "**B**" - `32:2B`
  * First portion is the value, second portion is the number of bytes needed to be allocated.
  * Bits will contain two parts separated by a "**:**" and ending with "**b**" - `9:5b`
  * First portion is the value, second portion is the number of bits needed to be allocated.

  * Stream type values can be of either decimal or hexadecimal numerals
  * Stream values in decimal must be separated by "**:**" - `12:255:90:42`
  * Stream values in hexadecimal must start with an "**x**" - `xfeda`

  * To access a packet member, the syntax must be as, <br> `{packet}.{name_of_member}`

  * Function calls will usually accept any already declared variables or raw values  or packet members <br> `print( {already_declared_variable} )`

  * Function calls can also accept strings, if the function can handle
  * Strings must be enclosed within double quotes '**"**' <br> `print("Have a Nice Day!")`

  * Functions can also have other functions as arguments <br> `print({any_other_function})`

<br>

### How to USE ding Shell:
  * Open ding Shell.
  * The shell prompt will be a word inside paranthesis, which indicates the current block you are in.
  `(_ding_)` or `(.decl:)` or `(.func:)` or `(.flow:)`

  * The `(_ding_)` block can be considered as a comment section.
  * Whatever is typed in the block will be ignored, except for the other block keywords or exit keyword.

  * To switch between blocks, type the corresponding block that you need to switch to.
  * For example, to switch to `.flow:`, type `.flow:` in the prompt. <br>

		(.decl:)
		(.decl:) .flow:
		
		(.flow:)
  * To exit the shell, type _exit_ in any block

