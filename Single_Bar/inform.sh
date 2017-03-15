#!/bin/sh
message=$1
ACCESS_TOKEN= #your ACCESS_TOKEN
curl -s -X POST -H "Authorization: Bearer $ACCESS_TOKEN" -F "message=$message" https://notify-api.line.me/api/notify
