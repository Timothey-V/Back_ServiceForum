module Channels: {
  type t;
  let make: (int, string) => t;
  let getId: t => int;
  let getCoursName: t => string;
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = {
    id: int,
    coursname: string,
  };

  let make = (id, coursname) => {id, coursname};

  let getId = channels => channels.id;
  let getCoursName = channels => channels.coursname;

  let fromJson = json =>
    Json.Decode.{
      id: json |> field("id", int),
      coursname: json |> field("coursname", string),
    };

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = channels =>
    Json.Encode.(
      object_([
        ("id", string(string_of_int(channels.id))),
        ("coursname", string(channels.coursname)),
      ])
    );
  let toString = channels => toJson(channels) |> Js.Json.stringify;
};

module Channelslist: {
  type t = list(Channels.t); // do not overuse abstraction when unecessary
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = list(Channels.t);

  let fromJson: Js.Json.t => t =
    json => json |> Json.Decode.(list(Channels.fromJson));

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = channelslist =>
    Array.of_list(channelslist)
    |> Array.map(item => Channels.toJson(item))
    |> Json.Encode.jsonArray;
  let toString = channelslist => toJson(channelslist) |> Js.Json.stringify;
};
