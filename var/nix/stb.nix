
{ stdenv, fetchFromGitHub }:

stdenv.mkDerivation rec {
  name = "stb-${version}";
  version = "955dfe991b26f6fb1287ec0093f606843487b099";

  src = fetchFromGitHub {
    owner = "nothings";
    repo = "stb";
    rev = version;
    sha256 = "0b7dvi7av0dk0pq5iczxx0f5ba1z1b5yvym6r1cv9dinqwbw6l2i";
  };

  installPhase = ''
    mkdir -p $out/include/stb
    cp -r * $out/include/stb
  '';

  meta = with stdenv.lib; {
    description = "Generic header util lib";
    homepage = "https://github.com/nothings/stb";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
    platforms = platforms.all;
  };
}

