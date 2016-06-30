{ stdenv
, fetchFromGitHub
, glfw3
, premake4
, pkgconfig
, mesa_glu
, glew
}:

stdenv.mkDerivation rec {
  name = "nanovg-git-${version}";
  version = "2016-02-01";

  src = fetchFromGitHub {
    owner = "memononen";
    repo = "nanovg";
    rev = "b83cf926525e7cea8d2483da2a75852b8c7b6d28";
    sha256 = "1zp355pw6xnb5mjarz67cxkglv7j1s0qzv705fz1wf0gl0sjhy8f";
  };

  configurePhase = ''
    premake4 gmake
    cd build
  '';

  makeFlags = "PREFIX=$(out)";

  installPhase = ''
    mkdir -p $out/{lib,include/nanovg}
    cp libnanovg.a $out/lib
    cp "../src/"* $out/include/nanovg
  '';

  buildInputs = [ mesa_glu pkgconfig premake4 glfw3 glew ];

  meta = with stdenv.lib; {
    description = "OpenGL antialiased 2D vector drawing library";
    homepage = "https://github.com/memononen/nanovg";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.zlib;
  };
}
