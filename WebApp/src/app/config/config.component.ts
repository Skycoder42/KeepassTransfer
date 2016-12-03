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
  @Output()
  public onConfigReady: EventEmitter<QrConfig> = new EventEmitter<QrConfig>();

  private keySizes: number[] = [512, 1024, 2048, 4096, 8192];
  private errorLevels: any[] = [
    {key:"Level L (Low)",value:'L'},
    {key:"Level M (Medium)",value:'M'},
    {key:"Level Q (Quartile)",value:'Q'},
    {key:"Level H (High)",value:'H'}
  ];

  private currentConfig: QrConfig = new QrConfig();
  private currentLink: string = null;

  public constructor(private route: ActivatedRoute, private router: Router) {
  }

  private onSubmit(): void {
    this.onConfigReady.emit(this.currentConfig);
  }

  private onCreateLink(): void {
    this.currentLink = this.router.createUrlTree(["/"], {
      queryParams: {
        keySize: this.currentConfig.keySize,
        errorLevel: this.currentConfig.errorLevel,
        qrSize: this.currentConfig.qrSize
      }
    }).toString();
  }
}
