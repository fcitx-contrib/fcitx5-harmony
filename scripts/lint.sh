set -e

has_crlf=0
while IFS= read -r file; do
  if file "$file" | grep -q "CRLF"; then
    has_crlf=1
    echo $file
  fi
done < <(git ls-files)

if [[ $has_crlf == 1 ]]; then
  echo "CRLF check failed"
  exit 1
fi

git ls-files '*.cpp' '*.h' | xargs clang-format -Werror --dry-run
