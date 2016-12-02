import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';

import { AppComponent } from './app.component';
import { HelpComponent } from './help/help.component';
import { ProgressComponent } from './progress/progress.component';
import { ProgressbarModule } from "ng2-bootstrap";

@NgModule({
  declarations: [
    AppComponent,
    HelpComponent,
    ProgressComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    ProgressbarModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
