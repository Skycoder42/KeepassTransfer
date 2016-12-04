# KeepassTransfer
A Plugin for [Keepass2Android](https://play.google.com/store/apps/details?id=keepass2android.keepass2android) to transfer entries to the browser

## General Idea
With Keepass Transfer, you can safly access your database entries from any browser, by using your Android app. With the plugin for Keepass2Android, you can select an entry, and then send it encrypted to your browser. There, by running the Web-App, your entry will be decrypted and you can access the fields.

## Usage
1. Download the [Plugin-App]() and enable it in the Keepass2Android.
2. Open the [Keepass Transfer Webapp](https://kpt.skycoder42.de/) and generate a key for the QR-Code
3. On Android, open a keepass database, choose an entry and select the "Transfer Entry" action
4. Choose the fields you want to transfer and then scan the QR-Code in your browser
5. A few moments later you will be able to access those fields from the browser!

## Requirements
The android app requires the [ZXing Barcode Scanner](https://play.google.com/store/apps/details?id=com.google.zxing.client.android). It uses the app to scan QR-Codes. If you don't already have it, you will be prompted to install it as soon as you try to scan a QR-Code.

## Project Structure
The project consists of 3 Sub-Projects.
- **Keepass.Transfer.Android:** A Xamarin Android application, the plugin for Keepass2Android
- **WebApp**: An Angular2-CLI web-application, which provides the browser part
- **TransferBackend**: A Qt Websocket server, which is responsible for the transfer of the entry between app and website

## Troubleshooting
- If you don't see the plugin action in Keepass2Android, make shure you followed these steps:
  - After installing, you opened the Keepass Transfer App once. It shows the settings (This is required for Android to actually "enable" the application on your phone)
  - Open Kp2A and go to settings -> plugins
  - There, select the Keepass Transfer Plugin, and activate it
- Sending the entries generates an error "key to small"
  - Just follow the instructions and create a bigger key on the website
