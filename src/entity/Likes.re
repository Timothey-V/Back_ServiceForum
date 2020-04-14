module Likes: {
  type t;
  let make: (int, string) => t;
  let getIdMessage: t => int;
  let getAuteur: t => string;
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = {
    idmessage: int,
    auteur: string,
  };

  let make = (idmessage, auteur) => {idmessage, auteur};

  let getIdMessage = likes => likes.idmessage;
  let getAuteur = likes => likes.auteur;

  let fromJson = json =>
    Json.Decode.{
      idmessage: json |> field("idmessage", int),
      auteur: json |> field("auteur", string),
    };

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = likes =>
    Json.Encode.(
      object_([
        ("idmessage", string(string_of_int(likes.idmessage))),
        ("auteur", string(likes.auteur)),
      ])
    );
  let toString = likes => toJson(likes) |> Js.Json.stringify;
};

module Likeslist: {
  type t = list(Likes.t); // do not overuse abstraction when unecessary
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = list(Likes.t);

  let fromJson: Js.Json.t => t =
    json => json |> Json.Decode.(list(Likes.fromJson));

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = likeslist =>
    Array.of_list(likeslist)
    |> Array.map(item => Likes.toJson(item))
    |> Json.Encode.jsonArray;
  let toString = likeslist => toJson(likeslist) |> Js.Json.stringify;
};
