#!/bin/bash

# --------
# toy-http
# --------
printf "\033c\n\033[31;1mtoy-http\033[0m\n"
ab -n150000 -c128 http://127.0.0.1:8976/index.html

# -------
# apache2
# -------
printf "\n\033[32;1mapache2\033[0m\n"
ab -n150000 -c128 http://127.0.0.1:8080/index.html

# -----
# nginx
# -----
printf "\n\033[32;1mnginx\033[0m\n"
ab -n150000 -c128 http://127.0.0.1:80/index.html
