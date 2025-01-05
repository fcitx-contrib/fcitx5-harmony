deps=(
  ecm
  fmt
  libintl
)

ARCH=$1
EXTRACT_DIR=entry/src/main/cpp/prebuilt/$ARCH
mkdir -p $EXTRACT_DIR

for dep in "${deps[@]}"; do
  file=$dep-$ARCH.tar.bz2
  [[ -f cache/$file ]] || wget -P cache https://github.com/fcitx-contrib/fcitx5-prebuilder/releases/download/harmony/$file
  tar xf cache/$file -C $EXTRACT_DIR
done
