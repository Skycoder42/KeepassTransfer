import { Component } from '@angular/core';
import { QrConfig } from "../qr-config";
import {NgControl} from "@angular/forms";
import {NumberValidator} from "./number-validator";

@Component({
  selector: 'app-help',
  templateUrl: './help.component.html',
  styleUrls: ['./help.component.css']
})
export class HelpComponent {
  public keySizes = [512, 1024, 2048, 4096, 8192];
  public errorLevels = [
    {key:"Level L (Low)",value:1},
    {key:"Level M (Medium)",value:0},
    {key:"Level Q (Quartile)",value:3},
    {key:"Level H (High)",value:2}
  ];

  public completed: boolean = false;
  public currentConfig: QrConfig = new QrConfig();

  onSubmit() {
    console.log(this.currentConfig.keySize);
    console.log(this.currentConfig.errorLevel);
    console.log(this.currentConfig.qrSize);
    this.completed = true;
  }
}
