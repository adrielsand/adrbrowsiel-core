@ECHO OFF
:: adrbrowsiel/script/redirect-cc.py is copied to %CD% and renamed to redirect.py in
:: the beginning of the build. See adrbrowsiel-browser/lib/util.js util.copyRedirectCC
python redirect.py %*
