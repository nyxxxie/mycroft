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
prepare() {
    mkdir -p build
}

build() {
    cd build
    cmake ../$pkgname \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    make
}
package() {
    cd build
    make DESTDIR="$pkgdir" install
}
