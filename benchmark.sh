#!/bin/bash

# --------
# toy-http
# --------
printf "\033c\n\033[31;1mtoy-http\033[0m\n"
ab -n200000 -c500 http://127.0.0.1:8000/index.html

# -------
# apache2
# -------
printf "\n\033[32;1mapache2\033[0m\n"
ab -n200000 -c500 http://127.0.0.1:80/index.html

# -----------
# http-server
# -----------
printf "\n\033[33;1mhttp-server (node)\033[0m\n"
ab -n200000 -c500 http://127.0.0.1:8080/index.html
