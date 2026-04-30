# QLdapEditor

LDAP Editor based on Qt6 and the ldapc++ wrapper.

![Screenshot](screenshot.png)

## Installation

### macOS

**Homebrew (recommended)**

```sh
brew tap tachyons/QLdapEditor https://github.com/tachyons/QLdapEditor
brew install --cask ldapeditorapp
```

**Direct download**

Download `LdapEditorApp-macos.dmg` from the [latest release](https://github.com/tachyons/QLdapEditor/releases/latest), open it, and drag `LdapEditorApp.app` to your Applications folder.

### Linux

**AppImage**

```sh
wget https://github.com/tachyons/QLdapEditor/releases/latest/download/LdapEditorApp-linux-x86_64.AppImage
chmod +x LdapEditorApp-linux-x86_64.AppImage
./LdapEditorApp-linux-x86_64.AppImage
```

**DEB package (Debian/Ubuntu)**

```sh
wget https://github.com/tachyons/QLdapEditor/releases/latest/download/ldapeditor-<version>.x86_64.deb
sudo apt install ./ldapeditor-<version>.x86_64.deb
```

---

## Building from source

### Requirements

- CMake 3.16+
- Qt 6.5+ with `Qt5Compat` module
- OpenLDAP development headers
- OpenSSL development headers

### macOS

```sh
brew install qt openldap openssl
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build --parallel
```

### Linux (Debian/Ubuntu)

```sh
sudo apt install libldap-dev libssl-dev libsasl2-dev \
  libgl1-mesa-dev libxcb-cursor0 libxkbcommon-x11-0
# Install Qt6 via https://github.com/jurplel/install-qt-action or your distro packages
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

---

## Feature list

### Connection to LDAP server
- Connect using Anonymous, Simple, or TLS authentication
- Show list of previous successful connections
- Save and reload last-used connection settings
- Reconnect to server

### Browse and edit LDAP tree
- Add, edit, and delete entries in the LDAP tree
- Refresh the LDAP tree
- Save or reject changes on the server
- Load the tree iteratively on expand

### Browse and edit LDAP attributes
- Add and delete attributes
- Colour-coded May/Must attribute display
- Icon indicator for editable data
- Filter and sort the attributes list

### Search LDAP data
- Set base DN for search
- Configure entry filter and attribute list to return

### Manage users and groups
- Create user entries
- Change user password (as administrator)
- Add/remove members in groups
- Lock/unlock user accounts with visual indicator

### Packaging
- macOS `.app` bundle and DMG
- Linux AppImage and DEB package
- XDG desktop integration (Linux)

---

[Fortress LDAP Schema and Config](https://github.com/apache/directory-fortress-core/tree/master/ldap) for reference.
