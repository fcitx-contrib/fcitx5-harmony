while IFS= read -r file; do
  if file "$file" | grep -q "CRLF"; then
    dos2unix $file
  fi
done < <(git ls-files)

git ls-files '*.cpp' '*.h' | xargs clang-format -i
