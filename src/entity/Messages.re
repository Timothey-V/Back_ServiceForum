module Messages: {
  type t;
  let make: (int, string, string, string, string, int) => t;
  let getIdMessage: t => int;
  let getTexte: t => string;
  let getDateEnvoi: t => string;
  let getIdCours: t => string;
  let getAuteur: t => string;
  let getNbLikes : t => int;
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = {
    idMessage: int,
    texte: string,
    dateEnvoi: string,
    idCours: string,
    auteur: string,
    nbLikes : int,
  };

  let make = (idMessage, texte, dateEnvoi, idCours, auteur, nbLikes) => {idMessage, texte, dateEnvoi, idCours, auteur, nbLikes};

  let getIdMessage = message => message.idMessage;
  let getTexte = message => message.texte;
  let getDateEnvoi = message => message.dateEnvoi;
  let getIdCours = message => message.idCours;
  let getAuteur = message => message.auteur;
  let getNbLikes = message => message.nbLikes;

  let fromJson = json =>
    Json.Decode.{
      idMessage: json |> field("idMessage", int),
      texte: json |> field("texte", string),
      dateEnvoi: json |> field("dateEnvoi", string),
      idCours: json |> field("idCours", string),
      auteur: json |> field("auteur", string),
      nbLikes: json |> field("nbLikes", int),
    };

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = message =>
    Json.Encode.(
      object_([
        ("idMessage", string(string_of_int(message.idMessage))),
        ("texte", string(message.texte)),
        ("dateEnvoi", string(message.dateEnvoi)),
        ("idCours", string(message.idCours)),
        ("auteur", string(message.auteur)),
        ("nbLikes", string(string_of_int(message.nbLikes))),
      ])
    );
  let toString = message => toJson(message) |> Js.Json.stringify;
};

module Messageslist: {
  type t = list(Messages.t); // do not overuse abstraction when unecessary
  let fromJson: Js.Json.t => t;
  let fromString: string => option(t);
  let toJson: t => Js.Json.t;
  let toString: t => string;
} = {
  type t = list(Messages.t);

  let fromJson: Js.Json.t => t =
    json => json |> Json.Decode.(list(Messages.fromJson));

  let fromString = jsonString =>
    switch (Json.parse(jsonString)) {
    | Some(validJson) => Some(fromJson(validJson))
    | None => None
    };

  let toJson = messageslist =>
    Array.of_list(messageslist)
    |> Array.map(item => Messages.toJson(item))
    |> Json.Encode.jsonArray;
  let toString = messageslist => toJson(messageslist) |> Js.Json.stringify;
};
