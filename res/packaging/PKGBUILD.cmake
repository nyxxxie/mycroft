# Maintainer: Nyxxie <nyxxie@protonmail.ch>
pkgname=${MYCROFT_NAME}
pkgver=${MYCROFT_VERSION}
pkgrel=1
pkgdesc="${MYCROFT_DESCRIPTION}"
url="github.com/nyxxxie/mycroft"
arch=('i686' 'x86_64')
license=('GPL')
depends=('sqlite' 'capstone' 'python' 'openssl' 'gtest' 'qt5-base')
optdepends=('graphviz: doxygen graph support')
makedepends=('make' 'cmake')
provides=('qmycroft' 'libmycroft_core')
conflicts=()
replaces=()
backup=()
source=("git+https://github.com/nyxxxie/mycroft.git")
sha256sums=('SKIP')
build() {
    cd "$srcdir/$pkgname"
    mkdir build || rm -rf build && mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="$srcdir/$pkgname" ..
    make || return 1
}
package() {
    cd "$srcdir/$pkgname/build"
    make install
}
