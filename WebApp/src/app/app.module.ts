import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { ConfigComponent } from './config/config.component';
import { ProgressbarModule } from "ng2-bootstrap";
import {RouterModule, Routes} from "@angular/router";
import {EncryptionService} from "./encryption-service";
import {QRCodeComponent} from "ng2-qrcode";

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
    RouterModule.forRoot(appRoutes)
  ],
  providers: [
    EncryptionService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
