import {Component, Input} from '@angular/core';
import {DataEntry} from "../data-entry";

@Component({
  selector: 'app-entry',
  templateUrl: './entry.component.html',
  styleUrls: ['./entry.component.css']
})
export class EntryComponent {
  private titleEntry: DataEntry;
  private formEntries: DataEntry[];

  @Input()
  public set entryList(entryList: DataEntry[]) {
    this.titleEntry = null;
    this.formEntries = [];
    for(let entry of entryList) {
      if(entry.key == "Title")
        this.titleEntry = entry;
      else
        this.formEntries.push(entry);
    }
  }

  private translateKey(key: string): string {
    switch (key) {
      case "UserName":
        return "Username";
      case "Password":
        return "Password";
      case "URL":
        return "URL";
      case "Notes":
        return "Notes";
      default:
        return key;
    }
  }
}
