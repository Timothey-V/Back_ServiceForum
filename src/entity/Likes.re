module Likes: {
  type t;
  let make: (int, int) => t;
  let getIdMessage: t => int;
  let getAuteur: t => int;
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = {
    idMessage: int,
    auteur: int,
  };

  let make = (idMessage, auteur) => {idMessage, auteur};

  let getIdMessage = likes => likes.idMessage;
  let getAuteur = likes => likes.auteur;

  let fromJson = json =>
    Json.Decode.{
      idMessage: json |> field("idMessage", int),
      auteur: json |> field("auteur", int),
    };

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = likes =>
    Json.Encode.(
      object_([
        ("idMessage", string(string_of_int(likes.idMessage))),
        ("auteur", string(string_of_int(likes.auteur))),
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
