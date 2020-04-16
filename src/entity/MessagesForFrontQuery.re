module MessagesForFrontQuery: {
  type t;
  let make: (string, int, list(MessagesForFront.MessagesForFront.t)) => t;
  let getCommand: t => string;
  let getRowCount: t => int;
  let getRows: t => list(MessagesForFront.MessagesForFront.t);
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
} = {
  type t = {
    command: string,
    rowCount: int,
    rows: list(MessagesForFront.MessagesForFront.t),

  };

  let make = (command, rowCount, rows) => {command, rowCount, rows};


  let getCommand = message => message.command;
  let getRowCount = message => message.rowCount;
  let getRows = message => message.rows;


  let fromJson = json =>
    Json.Decode.{
      command: json |> field("command", string),
      rowCount: json |> field("rowCount", int),
      rows: json |> field("rows", list(MessagesForFront.MessagesForFront.fromJson)),
    };

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };


};


module MessagesForFrontQueryList: {
  type t = list(MessagesForFrontQuery.t); // do not overuse abstraction when unecessary
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
} = {
  type t = list(MessagesForFrontQuery.t);

  let fromJson: Js.Json.t => t =
    json => json |> Json.Decode.(list(MessagesForFrontQuery.fromJson));

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };


};
