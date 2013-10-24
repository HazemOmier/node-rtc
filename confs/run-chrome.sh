rm -rf /tmp/chrome-prefs
cp -rf confs/chrome-prefs /tmp
exec google-chrome --user-data-dir=/tmp/chrome-prefs --no-default-browser-check --no-first-run --disable-default-apps --use-fake-ui-for-media-stream --no-sandbox "$@"
