deps=(
  boost
  ecm
  fmt
  lua
  libintl
  marisa
  opencc
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
  for dir in lib share; do
    if tar tf "cache/$file" | grep -qE "^$dir($|/)"; then
      tar xf cache/$file -C $RES_EXTRACT_DIR $dir
    fi
  done
done
