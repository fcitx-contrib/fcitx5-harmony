set -e

deps=(
  boost
  curl
  ecm
  iso-codes
  json
  json-c
  libexpat
  libintl
  libxkbcommon
  lua
  marisa
  opencc
  openssl
  xkeyboard-config
  zstd
)

ARCH=$1
EXTRACT_DIR=entry/src/main/cpp/prebuilt/$ARCH/usr
mkdir -p $EXTRACT_DIR

for dep in "${deps[@]}"; do
  file=$dep-$ARCH.tar.bz2
  [[ -f cache/$file ]] || wget -P cache https://github.com/fcitx-contrib/fcitx5-prebuilder/releases/download/harmony/$file
  tar xf cache/$file -C $EXTRACT_DIR
done

RES_EXTRACT_DIR=entry/src/main/resources/resfile/usr
mkdir -p $RES_EXTRACT_DIR

addons=(
  chinese-addons
  lua
)
for addon in "${addons[@]}"; do
  file=$addon-any.tar.bz2
  [[ -f cache/$file ]] || wget -P cache https://github.com/fcitx-contrib/fcitx5-plugins/releases/download/macos-latest/$file
  tar xf cache/$file -C $RES_EXTRACT_DIR lib || true
  tar xf cache/$file -C $RES_EXTRACT_DIR share
done

RAW_DIR=entry/src/main/resources/rawfile
mkdir -p $RAW_DIR
wget -O $RAW_DIR/index.html https://github.com/fcitx-contrib/fcitx5-keyboard-web/releases/download/harmony/index.html
