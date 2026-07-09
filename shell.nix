{ pkgs ? import <nixpkgs> {} }:
let
  mingw = pkgs.pkgsCross.mingwW64;
in
  pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
      gcc
      glfw
      xorg.libX11
      xorg.libXrandr
      xorg.libXi
      xorg.libXxf86vm
      xorg.libXcursor
      libGL
      mingw.buildPackages.gcc
    ];

    buildInputs = [
      mingw.glfw
    ];

    shellHook = ''
      export MINGW_GLFW=${mingw.glfw}
    '';
}
