projects=(C Java Go Rust)

# Building projects
for project in "${projects[@]}"; do
    echo "Building ${project}"
    cd "${project}"
    ./build.sh
    cd ..
done