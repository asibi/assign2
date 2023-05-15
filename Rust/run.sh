set -e
chmod +x ./target/x86_64-unknown-linux-musl/release/merge_sort
./target/x86_64-unknown-linux-musl/release/merge_sort "$1" "$2"
# cargo run --quiet --release "$1" "$2"