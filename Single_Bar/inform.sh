message=$1
ACCESS_TOKEN=43HfNSNajz0JhKhVahqBvqdEeyewhQKMNE1yzoCvYoz
curl -s -X POST -H "Authorization: Bearer $ACCESS_TOKEN" -F "message=$message" https://notify-api.line.me/api/notify
