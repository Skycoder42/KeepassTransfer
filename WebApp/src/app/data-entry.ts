export class DataEntry {
  public key: string;
  public value: string;
  public guarded: boolean;
  public encrypted: boolean;

  public visible: boolean;//helper property

  public constructor(data: any) {
    this.key = data.Key;
    this.value = data.Value;
    this.guarded = data.Guarded;
    this.encrypted = data.Encrypted;
    this.visible = !this.encrypted;
  }
}
