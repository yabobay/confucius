all: rust
	gcc *.c -o confucius \
	-ldiscord -lcurl \
	-Ltranslation/target/release -ltranslation -Wl,-rpath=translation/target/release -Wno-discarded-qualifiers

rust:
	cargo -Z unstable-options -C translation build --release
