import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { ConfigComponent } from './config/config.component';
import {ProgressbarModule} from "ng2-bootstrap";
import {RouterModule, Routes} from "@angular/router";
import {EncryptionService} from "./encryption-service";
import {QRCodeComponent} from "ng2-qrcode";
import {TransferService} from "./transfer-service";
import {ModalModule} from "angular2-modal";
import {BootstrapModalModule} from "angular2-modal/plugins/bootstrap";

const appRoutes: Routes = [];

@NgModule({
  declarations: [
    AppComponent,
    ConfigComponent,
    QRCodeComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    ProgressbarModule,
    BootstrapModalModule,
    ModalModule.forRoot(),
    RouterModule.forRoot(appRoutes)
  ],
  providers: [
    EncryptionService,
    TransferService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
