set -e

deps=(
  boost
  curl
  ecm
  fmt
  lua
  libintl
  marisa
  opencc
  openssl
  zstd
)

ARCH=$1
EXTRACT_DIR=entry/src/main/cpp/prebuilt/$ARCH
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
  [[ -f cache/$file ]] || wget -P cache https://github.com/fcitx-contrib/fcitx5-plugins/releases/download/macos/$file
  tar xf cache/$file -C $RES_EXTRACT_DIR lib || true
  tar xf cache/$file -C $RES_EXTRACT_DIR share
done
