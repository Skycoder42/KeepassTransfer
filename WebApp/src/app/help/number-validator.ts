import {NgControl} from "@angular/forms";

export interface ValidationResult {
  [key:string]:boolean;
}

export class NumberValidator {
  static isNumber(control: NgControl) : ValidationResult {
    if(isNaN(control.value))
      return {"noNumber":true};
    else
      return null;
  }
}
