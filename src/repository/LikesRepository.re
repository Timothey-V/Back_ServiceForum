open DbConnexion;

module Likes = {
  
  let delete = (idMessage, auteur) => {
    let likes = Likes.Likes.make(idMessage, auteur);
    Js.Promise.(
      Dbconnexion.knex
      |> Knex.rawBinding(
            "DELETE FROM forum_likes where idmessage = ? and auteur = ?",
            (
            Likes.Likes.getIdMessage(likes),
            Likes.Likes.getAuteur(likes),
            ),
          )
      |> Knex.toPromise
      |> then_(_ => {resolve()})
    );
  };
  
  let deleteAllLikesOfAMessage = (idMessage) => {
    let likes = Likes.Likes.make(idMessage, "auteur");
    Js.Promise.(
      Dbconnexion.knex
      |> Knex.rawBinding(
            "DELETE FROM forum_likes where idmessage = ? ",
            (
            Likes.Likes.getIdMessage(likes),
            ),
          )
      |> Knex.toPromise
      |> then_(_ => {resolve()})
    );
  };

  let isLikedByTheUser = (idMessage, auteur) => {
    let likes = Likes.Likes.make(idMessage, auteur);
    Js.Promise.(
      Dbconnexion.knex
      |> Knex.rawBinding(
            "select count(idmessage) FROM forum_likes where idmessage = ? and auteur = ? ",
            (
            Likes.Likes.getIdMessage(likes),
            Likes.Likes.getAuteur(likes),
            ),
          )
      |> Knex.toPromise
      |> then_(_ => {resolve()})
    );
  };


  let create = ( idMesssage, auteur) => {
      let likes = Likes.Likes.make(idMesssage, auteur);
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.rawBinding(
             "INSERT INTO forum_likes VALUES (? , ?)",
             (
              Likes.Likes.getIdMessage(likes),
              Likes.Likes.getAuteur(likes),
             ),
           )
        |> Knex.toPromise
        |> then_(_ => {resolve()})
      );
    };


};

