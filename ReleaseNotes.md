#	Release Notes - ding_u2.1

**Developer & Maintainer** - [Rishi Tharun](https://www.github.com/rishitharun) - <<vrishitharunj@gmail.com>><br>
<br>

> Release Date: `21.03.2021`

<br>

Key Points about ding_u2.1
--------------------------
* ding is still under development - **ding_u**
* ding is upgraded to Branch 2 - **ding_u2**
* This is release 1 of Branch 2 - **ding_u2.1**
* ding2 is completely incompatible with ding1
* ding1 is deprecated, and will be left in its incomplete form
* Support will not be given for ding1 henceforth

<br>

Changes made/Features added in this Release (internal):
-------------------------------------------------------
* ding Shell has been implemented
* Sourcecode Parsing Techique has been completely revised
* Replaced all string comparison based parsing technique of ding1 with tree-based parsing technique in ding2
* This release is intended to have all features upto *ding_u1.3*, only with parsing technique changes and shell implementation

Changes made/Features added in this Release (code prespective):
---------------------------------------------------------------
* `$INTERFACE` block has been deprecated. Instead, an interface function is introduced <br> `interface({interface_str})`
* Qualifiers have been deprecated.
* Method to assign a member has been changed <br>  ~~`{qualifier} {value/variable} -> {packet}:{member}`~~ &rightarrow; `{packet}.{member} = {value/variable}`
* Method to access a member has been changed <br> ~~`{qualifier} {packet}:{member}`~~ &rightarrow; `{packet}.{member}`
* NumStream values are henceforth separated by _colon_ "**:**", not by _underscore_ "**_**" <br> ~~`1_2_3_4`~~ &rightarrow; `1:2:3:4`
* `number` type values will henceforth have a "**B**" at the end, to specify that the size is of _bytes_ <br> ~~`32:3`~~ &rightarrow; `32:3B`
* `bit` type values will henceforth have a "**b**" at the end, to specify that the size is of _bits_ <br> ~~`32:6`~~ &rightarrow; `32:6b`

