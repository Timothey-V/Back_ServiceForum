open Express;

let welcome =
  Middleware.from((_next, _req) => {
    Json.Encode.(
      object_([("text", string("Welcome to 3ServiceForum API"))])
    )
    |> Response.sendJson
  });

let logRequest =
  Middleware.from((next, req) => {
    Request.ip(req)
    ++ " "
    ++ Request.methodRaw(req)
    ++ " ressource "
    ++ Request.path(req)
    |> Logger.info;
    next(Next.middleware);
  });

let badRessource =
  Middleware.from((_next, _req, rep) =>
    rep |> Response.sendStatus(NotFound)
  );
