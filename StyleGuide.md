## 自分用スタイルガイド

### Naming Rule

- common variable, namespace : Use snake_case

- data members : Use snake_case followed by an underscore "_"

- class, function : Use PascalCase

these method is written by snake_case for standard library
`begin / end`, `cbegin / cend`, `rbegin / rend`, `size`, `empty`, `clear`, `insert`, `erase`, `what`

these method is written by snake_case
`get_*`, `set_*`, `to_*`

### Line length

80 characters is the maximum.

A line may exceed 80 characters if it is a string literal that cannot easily be wrapped at 80 columns.
