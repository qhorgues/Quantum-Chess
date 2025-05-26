{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [
      clang-tools cmakeWithGui gdb gcc gnumake ];
    # packages = with pkgs; [  ];

}
