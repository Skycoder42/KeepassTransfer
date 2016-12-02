import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-progress',
  templateUrl: './progress.component.html',
  styleUrls: ['./progress.component.css']
})
export class ProgressComponent {
  public title: string = "Loading, please wait…"
  public description: string = "Please wait until Keepass Transfer is ready. It won't take long!"
}
