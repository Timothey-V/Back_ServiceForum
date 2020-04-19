open DbConnexion;

module Messages = {

      let getAllByCours : (string, string)  => Js.Promise.t(Js.Json.t) = (cours, auteur) =>
          Js.Promise.(    
            Dbconnexion.knex  
            |> Knex.rawBinding(
              "select res11.idmessage, texte, dateenvoi, idcours, auteur::text, nblikes::integer, coalesce(isliked,0)::integer as isliked from (
                select idmessage, texte, dateenvoi, idcours, auteur, coalesce(count(idmessageLike),0) as nblikes
                from (
                select fm.idmessage, texte,  TO_CHAR(dateenvoi,'YYYY/MM/DD HH24:MI:SS') as dateenvoi, idcours, fm.auteur, fl.idmessage as idmessageLike, coalesce(fl.auteur,0) as auteurLike
                from forum_messages fm
                left join forum_likes fl on fm.idmessage = fl.idmessage 
                where fm.idcours = ? ) res1
                group by idmessage, texte, dateenvoi, idcours, auteur ) res11
                full join 
                (select count(fl.auteur) as isliked, fl.idmessage 
                from forum_messages fm 
                left join forum_likes fl on fm.idmessage = fl.idmessage 
                where fm.idcours = ? and fl.auteur = ? 
                group by fl.idmessage ) res2 on res11.idmessage = res2.idmessage order by dateenvoi desc",
              (
                cours,
                cours,
               auteur,
              ),
            )
            |> Knex.toPromise
            |> then_(results => {
                  MessagesForFrontQuery.MessagesForFrontQuery.fromJson(results)
                  |> MessagesForFrontQuery.MessagesForFrontQuery.getRows
                  |> List.map(messages => {
                    MessagesForFront.MessagesForFront.make(
                      MessagesForFront.MessagesForFront.getIdMessage(messages),
                      MessagesForFront.MessagesForFront.getTexte(messages),
                      MessagesForFront.MessagesForFront.getDateEnvoi(messages),
                      MessagesForFront.MessagesForFront.getIdCours(messages),
                      MessagesForFront.MessagesForFront.getAuteur(messages),
                      MessagesForFront.MessagesForFront.getNbLikes(messages),
                      MessagesForFront.MessagesForFront.isLiked(messages)                        
                      )
                    })
                  |> MessagesForFront.MessagesForFrontlist.toJson
                  |> resolve
                  
                })
          );



      let deleteMessage = (idMessage) => {
        let messages = Messages.Messages.make(idMessage, "texte", Js.Date.toISOString(Js.Date.make()), "idCours", "auteur",0);
        ignore(LikesRepository.Likes.deleteAllLikesOfAMessage(idMessage));
        Js.Promise.(
          Dbconnexion.knex
          |> Knex.rawBinding(
               "DELETE FROM forum_messages where idmessage = ?",
               (
                Messages.Messages.getIdMessage(messages),
               ),
             )
          |> Knex.toPromise
          |> then_(_ => {resolve()})
        );
      };
  
  


      /*
  let decodeInt = json =>
      switch (Js.Json.classify(json)) {
        | Js.Json.JSONNumber(float) => Ok(int_of_float(float))
        | _ => Error("Not an int")
    };

  let handleValue =
  Js.Promise.then_(
    (result) =>
      Js.Promise.resolve(
        switch result {
        | Ok(value) => value
        | Error(reason) => -1
        }
      )
  );


      
  let getNextIdInBase = ()  =>
  Js.Promise.(
    Dbconnexion.knex
    |> Knex.raw("select max(idMessage) as value from forum_messages")
    |> then_(json => {
                 let myInt = decodeInt(json); 
                 /* do something with myInt*/
                 resolve();/* ici ça retourne Promise(unit) mais tu peux retourner n'importe quelle Promise('a) selon le type attendu en retour */
                 })
  );

*/
  
  let create = ( texte, idCours, auteur) => {
      let messages = Messages.Messages.make(0, texte, Js.Date.toISOString(Js.Date.make()), idCours, auteur,0);
      Js.Promise.(
        Dbconnexion.knex
        |> Knex.rawBinding(
             "INSERT INTO forum_messages VALUES ( (select max(idMessage)+1 from forum_messages) , ? , ? , ? , ?)",
             (
              Messages.Messages.getTexte(messages),
              Messages.Messages.getDateEnvoi(messages),
              Messages.Messages.getIdCours(messages),
              Messages.Messages.getAuteur(messages),
             ),
           )
        |> Knex.toPromise
        |> then_(_ => {resolve()})
      );
    };


};

