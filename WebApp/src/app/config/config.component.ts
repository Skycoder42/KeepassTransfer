import {Component, Output, EventEmitter} from '@angular/core';
import { QrConfig } from "../qr-config";
import {ActivatedRoute, Router, UrlTree} from "@angular/router";

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

  private currentConfig: QrConfig = QrConfig.fromQuery(document.location.search);
  private currentLink: string = null;

  private onSubmit(): void {
    this.onConfigReady.emit(this.currentConfig);
  }

  private onCreateLink(): void {
    let url = new URL(document.location.href);
    url.search = this.currentConfig.toQuery();
    this.currentLink = url.href;
  }
}
