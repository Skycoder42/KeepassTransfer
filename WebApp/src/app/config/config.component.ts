import {Component, Output, EventEmitter} from '@angular/core';
import { QrConfig } from "../qr-config";
import {ActivatedRoute, Router} from "@angular/router";
import {EncryptionService} from "../encryption-service";

@Component({
  selector: 'app-config',
  templateUrl: 'config.component.html',
  styleUrls: ['config.component.css']
})
export class ConfigComponent {
  @Output() onConfigReady = new EventEmitter<QrConfig>();

  public keySizes = [512, 1024, 2048, 4096, 8192];
  public errorLevels = [
    {key:"Level L (Low)",value:'L'},
    {key:"Level M (Medium)",value:'M'},
    {key:"Level Q (Quartile)",value:'Q'},
    {key:"Level H (High)",value:'H'}
  ];

  public currentConfig: QrConfig = new QrConfig();
  public currentLink: string = null;

  constructor(private route: ActivatedRoute, private router: Router) {
  }

  onSubmit() {
    this.onConfigReady.emit(this.currentConfig);
  }

  onCreateLink() {
    this.currentLink = this.router.createUrlTree(["/"], {
      queryParams: {
        keySize: this.currentConfig.keySize,
        errorLevel: this.currentConfig.errorLevel,
        qrSize: this.currentConfig.qrSize
      }
    }).toString();
  }
}
