cask "ldapeditorapp" do
  version "0.5.57"
  sha256 :no_check

  url "https://github.com/Shcherbich/QLdapEditor/releases/download/v#{version}/LdapEditorApp-macos.dmg"
  name "LDAP Editor"
  desc "GUI tool for viewing and editing LDAP directories"
  homepage "https://github.com/Shcherbich/QLdapEditor"

  app "LdapEditorApp.app"

  zap trash: [
    "~/Library/Preferences/com.ldapeditor.app.plist",
    "~/Library/Application Support/LdapEditorApp",
  ]
end
