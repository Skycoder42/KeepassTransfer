import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { ConfigComponent } from './config/config.component';
import {EncryptionService} from "./encryption-service";
import {QRCodeComponent} from "ng2-qrcode";
import {TransferService} from "./transfer-service";
import {ModalModule} from "angular2-modal";
import {BootstrapModalModule} from "angular2-modal/plugins/bootstrap";
import { EntryComponent } from './entry/entry.component';
import {ClipboardModule} from "angular2-clipboard";

@NgModule({
  declarations: [
    AppComponent,
    ConfigComponent,
    QRCodeComponent,
    EntryComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    BootstrapModalModule,
    ClipboardModule,
    ModalModule.forRoot()
  ],
  providers: [
    EncryptionService,
    TransferService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
