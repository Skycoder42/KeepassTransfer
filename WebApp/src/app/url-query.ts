export class UrlQuery {
  public query: string[] = [];

  public constructor(query: string = document.location.search) {
    let match,
      pl     = /\+/g,  // Regex for replacing addition symbol with a space
      search = /([^&=]+)=?([^&]*)/g,
      decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
      sQuery  = query.substring(1);

    while (match = search.exec(sQuery))
      this.query[decode(match[1])] = decode(match[2]);
  }
}
